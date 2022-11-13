import express from "express";
import bodyParser from "body-parser";
import { resolve } from "node:path";
import { Server } from "socket.io";
import { dirname } from "node:path";
import { fileURLToPath } from "url";
import { createServer } from "http";
import {
  getCurrentUser,
  getDrawerByRoom,
  getRoomUsers,
  selectDrawer,
  userJoin,
  userLeave,
} from "./utils/users.js";

const __dirname = dirname(fileURLToPath(import.meta.url));
let matrix;
let timeLeft = 60;
const startRound = new Map();

let prompt = "";
const app = express();

app.use(bodyParser.text());
app.use(
  bodyParser.urlencoded({
    extended: true,
  })
);

app.use(express.static("public"));
app.use(express.static("./"));

app.get("/", (req, res) => {
  res.sendFile(resolve(__dirname, "../public/html/index.html"));
});

app.get("/test", (req, res) => {
  console.log("TEST SUCCESS");

  const resp = { name: "Ballmeida", role: "bozo" };

  return res.send(resp);
});

app.get("/start", (req, res) => {
  console.log("TEST SUCCESS");
  if (startRound.get()) return res.send(resp);
});

app.post("/matrix", (req, res) => {
  console.log("MATRIX: ");
  console.log(req.body);
  matrix = JSON.parse(req.body);
  return res.send(200);
});

const httpServer = createServer(app);
const io = new Server(httpServer, {});

io.on("connection", (socket) => {
  console.log("Connected user id: " + socket.id);

  socket.on("guess", (guess) => {
    const user = getCurrentUser(socket.id);

    io.to(user.room).emit("message", formatMessage(user.username, guess));
  });

  socket.on("startRound", () => {
    const user = getCurrentUser(socket.id);
    // console.log(user);
    if (!startRound.get(user.room)) {
      socket.broadcast
        .to(user.room)
        .emit("moderator-message", "Round Starting");
      startRound.set(user.room, true);
      const drawer = selectDrawer(user.room);

      const prompt = selectPrompt();

      if (drawer) {
        socket.broadcast
          .to(user.room)
          .emit("moderator-message", `${drawer.username} is drawing`);
        io.to(drawer.id).emit(
          "moderator-message",
          "You are drawing: " + prompt
        );
      }

      io.to(user.room).emit("started");
      startTimer();
      function startTimer() {
        timeLeft = 60;
        function countdown() {
          timeLeft--;
          io.to(user.room).emit("timer", {
            timeLeft: timeLeft,
            matrix: matrix,
          });
          if (timeLeft > 0) {
            setTimeout(countdown, 1000);
            io.to(user.room).emit("round-over", {
              timeLeft: timeLeft,
              matrix: matrix,
            });
          }
        }

        setTimeout(countdown, 1000);
      }
    }
  });

  socket.on("joinRoom", ({ username, room }) => {
    const user = userJoin(socket.id, username, room);
    if (!startRound.has(room)) {
      startRound.set(room, false);
    } else {
      if (startRound.get(room)) {
        const drawer = getDrawerByRoom(user.room);
        if (drawer) {
          socket.emit("moderator-message", drawer.username + " is drawing");
        }
      }
    }
    socket.join(user.room);

    // Broadcast when a user connects
    socket.broadcast
      .to(user.room)
      .emit("moderator-message", `${user.username} has joined the game`);

    // Send users and room info
    io.to(user.room).emit("roomUsers", {
      room: user.room,
      users: getRoomUsers(user.room),
    });
  });

  socket.on("restart", () => {
    console.log("restart");
  });

  socket.on("disconnect", () => {
    const user = userLeave(socket.id);

    if (user) {
      io.to(user.room).emit(
        "moderator-message",
        `${user.username} has left the game`
      );

      const users = getRoomUsers(user.room);

      if (users.length === 0) {
        startRound.delete(user.room);
        timeLeft = 0;
      }

      // Send users and room info
      io.to(user.room).emit("roomUsers", {
        room: user.room,
        users: users,
      });
    }

    console.log("user " + socket.id + " left the game");
  });
});

function formatMessage(username, text) {
  return { username: username, text: text };
}

function selectPrompt() {
  return "dog";
}

httpServer.listen(process.env.PORT || 3000);
