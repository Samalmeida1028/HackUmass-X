import { io } from "https://cdn.socket.io/4.4.1/socket.io.esm.min.js";

const guessForm = document.getElementById("guess-form");
const userList = document.getElementById("users");
const socket = io("ws://localhost:3000");

let username = localStorage.getItem("username");

if (!username) {
  username = window.prompt("Enter your username", "Username");

  if (!username || username === "") {
    username = "Username";
  }

  //   localStorage.setItem("username", username);
}
const room = "baseRoom";

socket.emit("joinRoom", { username, room });

socket.on("roomUsers", ({ room, users }) => {
  outputUsers(users);
});

socket.on("moderator-message", (message) => {
  console.log(message);
  outputModMessage(message);
});

socket.on("message", (message) => {
  console.log(message);
  outputMessage(message);
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

// Add users to DOM
function outputUsers(users) {
  userList.innerHTML = "";
  users.forEach((user) => {
    const li = document.createElement("li");
    li.innerText = user.username;
    userList.appendChild(li);
  });
}
