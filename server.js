const http = require('http');

const server = new http.Server();
var counter = 1;

server.on('request', (req, res) => {
  res.end("Hello world!" + counter++);
});

server.listen(8080);

console.log("Listen on localhost:8080");
