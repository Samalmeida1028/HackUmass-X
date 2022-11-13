import { io } from "https://cdn.socket.io/4.4.1/socket.io.esm.min.js";

const guessForm = document.getElementById("guess-form");
const startButton = document.getElementById("start-button");
const timer = document.getElementById("timer");
const restartButton = document.getElementById("restart-button");
const userList = document.getElementById("users");
// const socket = io("ws://68.183.25.122:3000");
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
  //   startTimer();
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

guessForm.addEventListener("submit", (e) => {
  e.preventDefault();

  let guess = e.target.elements.guess.value;
  guess = guess.trim();
  console.log(guess);

  if (!guess) {
    return false;
  }

  socket.emit("guess", guess);

  e.target.elements.guess.value = "";
  e.target.elements.guess.focus();
});

function outputMessage(message) {
  const div = document.createElement("div");
  //   div.classList.add("message");
  const p = document.createElement("p");
  p.classList.add("meta");
  p.innerText = message.username;

  div.appendChild(p);
  const para = document.createElement("p");
  para.classList.add("text");
  para.innerText = message.text;
  div.appendChild(para);
  document.getElementById("chat-box").appendChild(div);
}

function outputModMessage(message) {
  const div = document.createElement("div");

  const para = document.createElement("p");
  para.classList.add("mod-text");
  para.innerText = message;
  div.appendChild(para);
  document.getElementById("chat-box").appendChild(div);
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
