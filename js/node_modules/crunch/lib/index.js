exports.Client = require('./client');

exports.createInstance = function(options) {
	return new exports.Client(options);
};
