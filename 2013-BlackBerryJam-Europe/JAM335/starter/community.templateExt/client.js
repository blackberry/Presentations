var _self = {},
	_ID = require("./manifest.json").namespace;

	// These methods are called by your App's JavaScript
	// They make WebWorks function calls to the methods
	// in the index.js of the Extension

	// Simple Synchronous test functions
	_self.startLED = function (hexcolor) {
		//
		//Incomplete
		//
	};

	_self.stopLED = function (id) {
		//
		//Incomplete
		//
	};

	// Define a property on the extension object
	Object.defineProperty(_self, "blinkCount", {
		get: function () {
			//
			//Incomplete
			//
		},
		set: function (arg) {
			//
			//Incomplete
			//
		}
	});

module.exports = _self;