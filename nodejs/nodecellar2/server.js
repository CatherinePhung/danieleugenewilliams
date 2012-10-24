var express = require('express'),
    wines   = require('./routes/wines');

var app = express();

app.configure(function() {
	app.use(express.logger('dev'));
	app.use(express.bodyParser());
});

app.get('/wines', wines.findAll);
app.get('/wines/:id', wines.findById);
app.post('/wines', wines.addWine);
app.put('wines/:id', wines.updateWine);
app.delete('/wines/:id', wines.deleteWine); 

app.listen(3000);
console.log('Listening on port 3000');
