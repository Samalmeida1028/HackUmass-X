const users = [];

// Join user to chat
export function userJoin(id, username, room) {
  const user = {
    id: id,
    username: username,
    room: room,
    hasDrawn: false,
    isDrawing: false,
  };

  users.push(user);

  return user;
}

// Get current user
export function getCurrentUser(id) {
  return users.find((user) => user.id === id);
}

// Get current user
export function selectDrawer(room) {
  console.log(users);

  const roomUsers = users.filter((user) => user.room === room);
  roomUsers.forEach((user) => {
    user.isDrawing = false;
  });
  const drawUsers = roomUsers.filter((user) => !user.hasDrawn);
  console.log(drawUsers);
  if (roomUsers.length === 0) {
    return undefined;
  }

  if (drawUsers.length > 0) {
    drawUsers[0].hasDrawn = true;
    drawUsers[0].isDrawing = true;
    return drawUsers[0];
  } else {
    roomUsers.forEach((user) => {
      user.hasDrawn = false;
      user.isDrawing = false;
    });
    roomUsers[0].hasDrawn = true;
    roomUsers[0].isDrawing = true;
    return roomUsers[0];
  }
}

// User leaves chat
export function userLeave(id) {
  const index = users.findIndex((user) => user.id === id);

  if (index !== -1) {
    return users.splice(index, 1)[0];
  }
}

// Get room users
export function getRoomUsers(room) {
  return users.filter((user) => user.room === room);
}

export function getDrawerByRoom(room) {
  return users.filter(
    (user) => user.room === room && user.isDrawing === true
  )[0];
}
