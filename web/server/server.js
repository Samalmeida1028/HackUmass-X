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
const totalTime = 60;
const prompts = [
  "dog",
  "cat",
  "tree",
  "ball",
  "man",
  "square",
  "egg",
  "stick",
  "sun",
  "earth",
  "house",
  "fish",
  "pizza",
];
let timeLeft = totalTime;
const startRound = new Map();
const room = "baseRoom";
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

// app.get("/time", (req, res) => {
//   console.log("TEST SUCCESS");

//   const resp = { time: 49 };

//   return res.send(resp);
// });

app.get("/start", (req, res) => {
  console.log("START");
  if (startRound.get(room)) {
    return res.send(prompt ? prompt : "ERROR");
  } else {
    return res.status(400).send("Not started");
  }
});

app.get("/prompts", (req, res) => {
  return res.send();
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
    if (prompt) {
      if (guess === prompt) {
        socket.emit("correct-guess");
        socket.emit("moderator-message", "You got the answer correct!");
        io.to(user.room).emit(
          "moderator-message",
          user.username + " got the answer!"
        );
      } else {
        io.to(user.room).emit("message", formatMessage(user.username, guess));
      }
    }
  });

  socket.on("startRound", () => {
    const user = getCurrentUser(socket.id);
    console.log("starting");

    if (!startRound.get(user.room)) {
      io.to(user.room).emit("moderator-message", "Round Starting");
      prompt = selectPrompt();
      startRound.set(user.room, true);
      const drawer = selectDrawer(user.room);

      if (drawer) {
        io.to(user.room).emit(
          "moderator-message",
          `${drawer.username} is drawing`
        );
        io.to(drawer.id).emit(
          "moderator-message",
          "You are drawing: " + prompt
        );
        io.to(drawer.id).emit("is-drawer");
      }

      io.to(user.room).emit("started");
      startTimer();
      function startTimer() {
        timeLeft = totalTime;

        function countdown() {
          timeLeft--;
          io.to(user.room).emit("timer", {
            timeLeft: timeLeft >= 0 ? timeLeft : 0,
            matrix: matrix,
          });
          if (timeLeft > 0) {
            setTimeout(countdown, 1000);
          } else {
            startRound.set(user.room, false);
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
    timeLeft = 0;
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
  return prompts[Math.floor(Math.random() * prompts.length)];
}

// function reset() {
//   prompt = undefined;
// }

// function generatePrompts() {
//   const numPrompts = 3;

//   const promptsCopy = [...prompts];
//   const generated = [];

//   for (let index = 0; index < numPrompts; index++) {
//     let i = Math.floor(Math.random() * promptsCopy.length);
//     const element = promptsCopy.splice(i, 1);
//     generated.push(element);
//   }

//   return generated;
// }

httpServer.listen(process.env.PORT || 3000);
