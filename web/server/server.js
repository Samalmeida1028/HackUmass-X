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
  getRoomUsers,
  userJoin,
  userLeave,
} from "./utils/users.js";

const __dirname = dirname(fileURLToPath(import.meta.url));

const app = express();

app.use(bodyParser.json());
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

  socket.on("startRound", ({ room }) => {
    const drawer = selectDrawer();
    const prompt = selectPrompt();
  });

  socket.on("joinRoom", ({ username, room }) => {
    const user = userJoin(socket.id, username, room);
    socket.join(user.room);

    // Broadcast when a user connects
    socket.broadcast
      .to(user.room)
      .emit("moderator-message", `${user.username} has joined the chat`);

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
        `${user.username} has left the chat`
      );

      // Send users and room info
      io.to(user.room).emit("roomUsers", {
        room: user.room,
        users: getRoomUsers(user.room),
      });
    }

    console.log("user " + socket.id + " left the game");
  });
});

function formatMessage(username, text) {
  return { username: username, text: text };
}

httpServer.listen(process.env.PORT || 3000);
