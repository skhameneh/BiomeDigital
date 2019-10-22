const http = require('http').createServer((req, res) => {
    console.log('request');
    res.end('Hello');
}).listen(3333, '0.0.0.0');
const io = require('socket.io').listen(http, {
    //'path': '/realtime',
});

io.on('connection', function (socket) {
    console.log('Connection!');

    socket.emit('time', Date.now());

    socket.on('ferret', (name, word, fn) => {
        fn(name + ' says ' + word);
    });

    socket.on('t', (data) => {
        console.log(data);
    });
});