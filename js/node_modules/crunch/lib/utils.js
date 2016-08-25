var Hoek = require('hoek');

for (var i in Hoek) {
	if (Hoek.hasOwnProperty(i)) {
		exports[i] = Hoek[i];
	}
}