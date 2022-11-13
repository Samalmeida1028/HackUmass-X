import { io } from "https://cdn.socket.io/4.4.1/socket.io.esm.min.js";

const guessButton = document.getElementById("guess-button");
const guessInput = document.getElementById("guess-input");
const startButton = document.getElementById("start-button");
const timer = document.getElementById("timer");
const restartButton = document.getElementById("restart-button");

const userList = document.getElementById("users");
// const socket = io("ws://localhost:3000");
const socket = io("ws://68.183.25.122:3000");

let username = undefined;

if (!username) {
  username = window.prompt("Enter your username", "Username");

  if (!username || username === "") {
    username = "User";
  }
}

const room = "baseRoom";
let started = false;

socket.emit("joinRoom", { username, room });

socket.on("roomUsers", ({ users }) => {
  outputUsers(users);
});

socket.on("started", () => {
  startButton.disabled = true;
  started = true;
});

socket.on("round-over", (resp) => {
  startButton.disabled = false;
  startButton.innerHTML = "Next Round";
  started = false;
  guessButton.innerHTML = "Guess";
  guessButton.disabled = false;
});

socket.on("timer", (resp) => {
  timer.innerHTML = String(resp.timeLeft);
  updateCanvas(resp.matrix);
});

socket.on("moderator-message", (message) => {
  console.log(message);
  outputModMessage(message);
});

socket.on("message", (message) => {
  console.log(message);
  outputMessage(message);
});

socket.on("correct-guess", (message) => {
  console.log("correct");
  guessButton.disabled = true;
  guessButton.innerHTML = "Correct!";
});

socket.on("is-drawer", () => {
  guessButton.disabled = true;
  guessButton.innerHTML = "Drawing";
});

restartButton.addEventListener("click", (e) => {
  e.preventDefault();

  socket.emit("restart");
});

startButton.addEventListener("click", (e) => {
  e.preventDefault();
  if (!started) {
    console.log("Start");
    socket.emit("startRound");
  } else {
    startButton.disabled = true;
  }
});

guessButton.addEventListener("click", (e) => {
  e.preventDefault();

  let guess = guessInput.value;
  guess = guess.trim();
  console.log(guess);

  if (!guess) {
    return false;
  }

  socket.emit("guess", guess);

  guessInput.value = "";
  guessInput.focus();
});

function outputMessage(message) {
  const div = document.createElement("div");
  //   div.classList.add("message");
  const p = document.createElement("p");
  p.classList.add("meta");
  p.innerText = message.username + ": " + message.text;

  div.appendChild(p);
  //   const para = document.createElement("p");
  //   para.classList.add("text");
  //   para.innerText = message.text;
  //   div.appendChild(para);
  const chatBox = document.getElementById("chat-box");
  chatBox.appendChild(div);
  chatBox.scrollTop = chatBox.scrollHeight;
}

function outputModMessage(message) {
  const div = document.createElement("div");

  const para = document.createElement("p");
  para.classList.add("mod-text");
  para.innerText = message;
  div.appendChild(para);
  const chatBox = document.getElementById("chat-box");
  chatBox.appendChild(div);
  chatBox.scrollTop = chatBox.scrollHeight;
}

function outputUsers(users) {
  userList.innerHTML = "";
  users.forEach((user) => {
    const li = document.createElement("li");
    li.innerText = user.username;
    userList.appendChild(li);
  });
}

function updateCanvas(matrix) {
  if (matrix === undefined) {
    return;
  }

  const canvas = document.getElementById("canvas");

  const ctx = canvas.getContext("2d");
  ctx.clearRect(0, 0, canvas.width, canvas.height);

  ctx.beginPath();

  matrix.forEach((pixel) => {
    if (pixel.hex !== "#ffffff") {
      ctx.fillStyle = pixel.hex;
    } else {
      ctx.fillStyle = "#000000";
    }
    ctx.fillRect(
      pixel.coord.y * 40,
      pixel.coord.x * 20,
      canvas.height / 40,
      canvas.width / 40
    );
  });
  ctx.closePath();
}
