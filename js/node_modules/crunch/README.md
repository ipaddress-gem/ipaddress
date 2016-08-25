crunch
======

v0.1.2

A queue crunching router / relay.  
This project is still pretty young.


[![Build Status](https://api.travis-ci.org/spectrumbranch/crunch.png)](https://travis-ci.org/spectrumbranch/crunch)


Developers
==========

To obtain and install, run: 

```
git clone git@github.com:spectrumbranch/crunch.git
cd crunch
npm install
```

To run tests:

```
make test
```

Usage
-----

To use crunch as a subscriber router/relay:

```
var Crunch = require('crunch');

var relay = {};

//These relay functions are where you put your custom logic.
relay.subscribe = function () {
	console.log('Will show when subscribed.');
}

relay.unsubscribe = function () {
	console.log('Will show when unsubscribed.');
}

relay.message = function (message) {
	console.log('The message sent on this channel is ' + message);
}
var crunch = Crunch.createInstance({ channel: 'myChannel', hooks: relay });

//Start listening
crunch.start();

//Use a redis client to publish some messages to the 'myChannel' channel.
//The relay.message function will be passed the message.

//To end the process
crunch.stop();
```

Early Phase Requirements (TODO)
------------------------

-Specify a channel with configuration for crunch to sub/pub to on redis.

-Messages are expected to be sent in valid parseable JSON. Both direct (value filled) and deferred (provides a 'memory address'-like redis key for another message) messages are understood.

__Example messages (aka command sets):__

A direct message:  
```{"cmd":"update", "type":"Tile", "x":0, "y": 16, "layer": 1, "tileid": 17, "mapid": 3, "lastChangedBy": 7 }```

A deferred message:  
```{"cmd":"queue", "key":"tileQueue01234"}```

If a router is passed into crunch's config, the router will pass all messages with `"cmd":"somename"` to the named router function "somename". If no router function has that name, the message is simply ignored. This allows a sort of plugin architecture on a single channel if the developer wishes to have one channel shared across multiple subscribers, each having their own job to do.
