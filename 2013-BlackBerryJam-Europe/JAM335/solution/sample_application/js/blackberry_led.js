var blackberry_led = (function(){

	/*
	 * Regexp to validate HEX colors.
	 * @constant
	 */
	var HEX_PATTERN = /^([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$/;

	// Default options
	var default_opts = {
		blinks: 3, // seconds
		color: 'FF00FF', // Magenta
	};
	/*
	 * @private
	 */
	var _id;
	/*
	 * @private
	 */
	var _blinks;
	/*
	 * @private
	 */
	var _color;

	var _message;

	var _blinksSelector;
	var _colorSelector;
	var _actionButton;
	var _colorPreview;

	/*
	 * Fuction to select default values
	 * @private
	 * @param element DOM object with all content of the page
	 */
	var init = function(element){

		_blinksSelector = element.getElementById('blinks');
		_colorSelector = element.getElementById('color');
		_actionButton = element.getElementById('actionButton');
		_colorPreview = element.getElementById('colorPreview');
		_output = element.getElementById('output');

		//Set default values
		_setDefaultValue(_blinksSelector, "blinks");
		_setDefaultValue(_colorSelector, "color");
		_previewColor();

		// Bind listeners
		color.addEventListener('change', _previewColor, false);
		// If extension is NOT ready returns false to avoid init call.
		if( _extensionReady() )
		{
			_actionButton.addEventListener('click', _actionLed,false);
		}
		else
		{
			_actionButton.disable();
		}
	};

	/*
	 * Fuction to select default values
	 * @private
	 * @param element DOM object to select the value
	 * @param option default_opts property for the selection
	 */
	var _setDefaultValue = function(element, option){
		var i = 0, found = false;
		while(!found && i < element.options.length)
		{
			found = element.options[i].value == default_opts[option];
			if( found )
			{
				element.setSelectedItem(i);
			}
			i++;
		}
	}

	/*
	 * Fuction to set color preview
	 * @private
	 */
	var _previewColor = function(){
		_colorPreview.style.backgroundColor = '#' + color.value;
	};

	/*
	 * Fuction to Start/Stop led blinking
	 * @private
	 */
	var _actionLed = function(){
		if( getId() )
		{
			stop();
		}
		else
		{
			setBlinks(_blinksSelector.value)
			setColor(_colorSelector.value)
			start();

		}
	};

	var _setAction = function(action){
		_actionButton.childNodes[0].childNodes[0].innerHTML = action;
	};

	var _getBlinkingTimeout = function(){
		// 300ms. blink time.
		// 3000ms. between blinks
		var blinks = getBlinks();
		return 300 * blinks + 3000 * (blinks-1);
	}

	/*
	 * Determine if the native is ready
	 * @private
	 * @param method Which method is checking the extension status
	 * @returns boolean true (it's ready) or false (it's NOT ready)
	 */
	var _extensionReady = function(){
		if (!window.community || !community.templateExt) {
			_message = "community.templateExt is undefined";
			console.error(_message);
			_log(_message);
			return false;
		}
		return true;
	};
	/*
	 * Is not a Color
	 * @private
	 * @param Color The color to check.
	 * @returns boolean true (it's NOT a color) or false (it's a color)
	 */
	var _isNaColor = function(color){
		return !HEX_PATTERN.test(color);
	};

	var _log = function(message) {
		var log = document.createElement("p");
		log.innerHTML = message;
		_output.insertBefore(log, _output.firstChild);
	}

	/*
	 * Set a new id.
	 * @private
	 * @param: id The new id
	 */
	var _setId = function(id){
		_id = id
	}

	/*
	 * @private
	 * Get the current id
	 */
	var getId = function(){
		return _id;
	};

	/*
	 * Set the number of blinks
	 * @param: blinks The blinks in seconds
	 */
	var setBlinks = function(blinks){
		if( isNaN(blinks) )
		{
			_message = blinks + " is not a valid number. It will use the default one: " + default_opts.blinks;
			console.warn(_message);
			_log(_message);
			_blinks = default_opts.blinks;
			return false;
		}
		_blinks = Number(blinks);
		_message = "[BLINKS] " + _blinks;
		console.info(_message);
		_log(_message);
		return true;
	};

	/*
	 * Get the number of blinks
	 */
	var getBlinks = function(){
		return _blinks;
	};

	/*
	 * Set a color
	 * @param: color The color in HEX (#RRGGBB or #RGB).
	 */
	var setColor = function(color){
		if( _isNaColor(color) )
		{
			_message = color + " is not a valid color. It will use the default one: " + default_opts.color;
			console.warn(_message);
			_log(_message);
			_color = default_opts.color;
			return false;
		}
		_color = color;
		_message = "[COLOR] " + _color;
		console.info(_message);
		_log(_message);
		return true;
	};

	/*
	 * Get the current color
	 */
	var getColor = function(){
		return _color;
	};

	/*
	 * Send the order to the native code to switch on the LED.
	 */
	var start = function(){
		//ADAM (Feb3)
		community.templateExt.blinkCount = _blinks;

		var id = getId();
		if (id)
		{
			_message = "[STOP] Id -> " + id;
			console.warn(_message);
			_log(_message);
			stop();
		}
		id = community.templateExt.startLED(getColor());
		setTimeout(stop, _getBlinkingTimeout());
		_setId(id);
		_setAction('Stop');
		_message = "[START] Id -> " + id;
		console.info(_message);
		_log(_message);
	};

	/*
	 * Send the order to the native code to switch off the LED.
	 */
	var stop = function(){
		var id = getId();
		community.templateExt.stopLED(id);
		_message = "[STOP] Id -> " + id;
		console.info(_message);
		_log(_message);
		_setId('');
		_setAction('Start');
	};

	return {
		init: init,
		default_opts: default_opts,
		getId: getId,
		setBlinks: setBlinks,
		getBlinks: getBlinks,
		setColor: setColor,
		getColor: getColor,
		start: start,
		stop: stop
	};
})();