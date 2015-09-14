var http = require('http'),
	path = require('path'),
	url = require('url'),
	fs = require('fs');

var config = {
	port: 8080,
};

var boundary = 'klas89o4y2wtrlinasdv8ocxwd';

http.createServer(function(req, res) {

	var uri = url.parse(req.url).pathname;

	if (uri == '/img.jpg') {

		// this has to be here otherwise node will write a content length before each iteration of the image update
		// you can also add a transfer encoding header which is null, but then the other header for multipart doesn't get sent
		res.useChunkedEncodingByDefault = false;

		// write headers for mjpeg with boundary
		res.writeHead(200, {'Content-type': 'multipart/x-mixed-replace; boundary=--'+boundary}, {'Cache-Control': 'no-cache'}, {'Connection': 'keep-alive'}, {'Pragma': 'no-cache'});

		var updateLoop = setInterval(function() {
			// write image data then boundary, repeat
			// get buffer of image
			var imgB = fs.readFileSync('y.jpg');

			res.write('Content-Type: image/jpeg\r\nContent-Length: '+(imgB.length)+'\r\n\r\n');
			// write to res
			res.write(imgB);
			// write boundary
			res.write('\r\n--'+boundary+'\r\n');
		}, 200);

		// this never ends because it is a stream
		//res.end();

		// handle close where browser closes it
		res.on('close', function() {
			clearInterval(updateLoop);
		});

	} else {

		// serve page to view image
		res.writeHead(200, {'Content-type': 'text/html'});
		res.end('<html><head><script>setInterval(function() {document.getElementById("rawr").src = "/img.jpg?"+Date.now();},1000);</script><title>FLIRCAM</title></head><body><img id="rawr" src="/img.jpg" width="800" height="600" /><body></html>');

	}

}).listen(config.port);

console.log('listening on port ' + config.port);
