var _self = {},
	_ID = require("./manifest.json").namespace;

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	// Simple Synchronous test functions
	_self.startLED = function (hexcolor) {
		var args = { hexcolor : hexcolor };
		return window.webworks.execSync(_ID, "startLED", args);

	};

	_self.stopLED = function (id) {
		var args = { id : id };
		return window.webworks.execSync(_ID, "stopLED", args);

	};

	// Define a property on the extension object
	Object.defineProperty(_self, "blinkCount", {
		get: function () {

			return window.webworks.execSync(_ID, "blinkCount", null);

		},
		set: function (arg) {

			window.webworks.execSync(_ID, "blinkCount", {"value": arg });

		}
	});

module.exports = _self;