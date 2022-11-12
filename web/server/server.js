import express from "express";
import bodyParser from "body-parser";
import { resolve } from "node:path";
import { Server } from "socket.io";
import { dirname } from "node:path";
import { fileURLToPath } from "url";
import { createServer } from "http";
import router from "./routes.js";
import {
  getCurrentUser,
  getDrawerByRoom,
  getRoomUsers,
  selectDrawer,
  userJoin,
  userLeave,
} from "./utils/users.js";

const __dirname = dirname(fileURLToPath(import.meta.url));

const startRound = new Map();

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

app.use("/", router);
app.get("/test", (req, res) => {
  console.log("TEST SUCCESS");

  const resp = { name: "Ballmeida", role: "bozo" };

  return res.send(resp);
});

app.post("/matrix", (req, res) => {
  console.log("MATRIX: ");
  console.log(req.body);

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

    socket.broadcast.to(user.room).emit("moderator-message", "Round Starting");
    startRound.set(user.room, true);
    const drawer = selectDrawer(user.room);
    console.log(drawer);
    const prompt = selectPrompt();
    socket.broadcast
      .to(user.room)
      .emit("moderator-message", `${drawer.username} is drawing`);
    io.to(drawer.id).emit("moderator-message", "You are drawing: " + prompt);
  });

  socket.on("joinRoom", ({ username, room }) => {
    const user = userJoin(socket.id, username, room);
    if (!startRound.has(room)) {
      startRound.set(room, false);
    } else {
      if (startRound.get(room)) {
        const drawer = getDrawerByRoom(user.room);

        socket.emit("moderator-message", drawer.username + " is drawing");
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
