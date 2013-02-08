var template = require("./templateJNEXT").template,
	_event = require("../../lib/event"),
	_utils = require("../../lib/utils");

module.exports = {

	startLED: function (success, fail, args) {
		hexcolor = JSON.parse(decodeURIComponent(args["hexcolor"]));
		//
		//Incomplete
		//
	},

	stopLED: function (success, fail, args) {
		id = JSON.parse(decodeURIComponent(args["id"]));
		//
		//Incomplete
		//
	},

	blinkCount: function (success, fail, args) {
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			//
			//Incomplete
			//
		} else {
			//
			//Incomplete
			//
		}
	}

};