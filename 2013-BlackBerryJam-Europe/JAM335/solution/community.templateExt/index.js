var template = require("./templateJNEXT").template,
	_event = require("../../lib/event"),
	_utils = require("../../lib/utils");

module.exports = {

	startLED: function (success, fail, args) {
		hexcolor = JSON.parse(decodeURIComponent(args["hexcolor"]));

		success(template.getInstance().startLED(hexcolor));

	},

	stopLED: function (success, fail, args) {
		id = JSON.parse(decodeURIComponent(args["id"]));

		success(template.getInstance().stopLED(id));

	},

	blinkCount: function (success, fail, args) {
		var value;
		if (args && args["value"]) {
			value = JSON.parse(decodeURIComponent(args["value"]));
			template.getInstance().blinkCount(value);
			success();

		} else {

			return success(template.getInstance().blinkCount());

		}
	}

};