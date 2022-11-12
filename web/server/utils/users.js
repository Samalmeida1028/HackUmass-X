const users = [];

// Join user to chat
export function userJoin(id, username, room) {
  const user = { id, username, room, hasDrawn };

  users.push(user);

  return user;
}

// Get current user
export function getCurrentUser(id) {
  return users.find((user) => user.id === id);
}

// Get current user
export function selectDrawer(room) {
  const roomUsers = users.filter((user) => user.room === room);
  const drawUsers = roomUsers.filter((user) => user.hasDrawn === false);

  if (roomUsers.length === 0) {
    return undefined;
  }

  if (drawUsers.length > 0) {
    drawUsers[0].hasDrawn = true;
    return drawUsers[0];
  } else {
    roomUsers.forEach((user) => {
      user.hasDrawn = false;
    });
    roomUsers[0].hasDrawn = true;
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
