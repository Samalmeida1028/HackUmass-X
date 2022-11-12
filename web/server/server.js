const express = require("express");
const bodyParser = require("body-parser");
const path = require("node:path");
const router = require("./routes");

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
  res.sendFile(path.resolve(__dirname, "../public/html/index.html"));
});

app.use("/", router);

app.listen(process.env.PORT || 3000);
