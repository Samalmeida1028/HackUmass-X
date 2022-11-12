import express from "express";
import bodyParser from "body-parser";
import { resolve } from "node:path";
import { Server } from "socket.io";
import { dirname } from "node:path";
import { fileURLToPath } from "url";
import { createServer } from "http";
import router from "./routes.js";

const __dirname = dirname(fileURLToPath(import.meta.url));

const app = express();

app.use(bodyParser.json());
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
  console.log(req.body);

  return res.send(200);
});

const httpServer = createServer(app);
const io = new Server(httpServer, {});

io.on("connection", (socket) => {
  console.log("Connected user id: " + socket.id);
});

httpServer.listen(process.env.PORT || 3000);

// app.listen(process.env.PORT || 3000);
