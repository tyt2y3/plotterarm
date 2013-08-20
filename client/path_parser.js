/**
Copyright (c) 2000-2004, Kevin Lindsey
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    - Neither the name of this software nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
define(function()
{

Svg.VERSION = 1.0;
Svg.NAMESPACE = "http://www.w3.org/2000/svg";

function Svg() {}
PathParser.PARAMCOUNT = {A: 7,C: 6,H: 1,L: 2,M: 2,Q: 4,S: 4,T: 2,V: 1,Z: 0};
PathParser.METHODNAME = {
	A: "arcAbs",
	a: "arcRel",
	C: "curvetoCubicAbs",
	c: "curvetoCubicRel",
	H: "linetoHorizontalAbs",
	h: "linetoHorizontalRel",
	L: "linetoAbs",
	l: "linetoRel",
	M: "movetoAbs",
	m: "movetoRel",
	Q: "curvetoQuadraticAbs",
	q: "curvetoQuadraticRel",
	S: "curvetoCubicSmoothAbs",
	s: "curvetoCubicSmoothRel",
	T: "curvetoQuadraticSmoothAbs",
	t: "curvetoQuadraticSmoothRel",
	V: "linetoVerticalAbs",
	v: "linetoVerticalRel",
	Z: "closePath",
	z: "closePath"
}

function PathParser() {
	this._lexer = new PathLexer();
	this._handler = null;
	this.result = [];
}
PathParser.prototype.parsePath = function (path) {
	if (path == null || path.namespaceURI != Svg.NAMESPACE || path.localName != "path") throw new Error("PathParser.parsePath: The first parameter must be an SVG path element");
	this.parseData(path.getAttributeNS(null, "d"));
};
PathParser.prototype.parseData = function (pathData) {
	if (typeof (pathData) != "string") throw new Error("PathParser.parseData: The first parameter must be a string");
	if (this._handler != null && this._handler.beginParse != null) this._handler.beginParse();
	var lexer = this._lexer;
	lexer.setPathData(pathData);
	var mode = "BOP";
	var token = lexer.getNextToken();
	while (!token.typeis(PathToken.EOD)) {
		var param_count;
		var params = new Array();
		switch (token.type) {
		case PathToken.COMMAND:
			if (mode == "BOP" && token.text != "M" && token.text != "m") throw new Error("PathParser.parseData: a path must begin with a moveto command");
			mode = token.text;
			param_count = PathParser.PARAMCOUNT[token.text.toUpperCase()];
			token = lexer.getNextToken();
			break;
		case PathToken.NUMBER:
			break;
		default:
			throw new Error("PathParser.parseData: unrecognized token type: " + token.type);
		}
		for (var i = 0; i < param_count; i++) {
			switch (token.type) {
			case PathToken.COMMAND:
				throw new Error("PathParser.parseData: parameter must be a number: " + token.text);
			case PathToken.NUMBER:
				params[i] = token.text - 0;
				break;
			default:
				throw new Errot("PathParser.parseData: unrecognized token type: " + token.type);
			}
			token = lexer.getNextToken();
		}
		if (this._handler != null) {
			var handler = this._handler;
			var method = PathParser.METHODNAME[mode];
			if (handler[method] != null) handler[method].apply(handler, params);
		}
		this.result.push([mode].concat(params));
		if (mode == "M") mode = "L";
		if (mode == "m") mode = "l";
	}
};
PathParser.prototype.setHandler = function (handler) {
	this._handler = handler;
};
PathLexer.VERSION = 1.0;

function PathLexer(pathData) {
	if (pathData == null) pathData = "";
	this.setPathData(pathData);
}
PathLexer.prototype.setPathData = function (pathData) {
	if (typeof (pathData) != "string") throw new Error("PathLexer.setPathData: The first parameter must be a string");
	this._pathData = pathData;
};
PathLexer.prototype.getNextToken = function () {
	var result = null;
	var d = this._pathData;
	while (result == null) {
		if (d == null || d == "") {
			result = new PathToken(PathToken.EOD, "");
		} else if (d.match(/^([ \t\r\n,]+)/)) {
			d = d.substr(RegExp.$1.length);
		} else if (d.match(/^([AaCcHhLlMmQqSsTtVvZz])/)) {
			result = new PathToken(PathToken.COMMAND, RegExp.$1);
			d = d.substr(RegExp.$1.length);
		} else if (d.match(/^(([-+]?[0-9]+(\.[0-9]*)?|[-+]?\.[0-9]+)([eE][-+]?[0-9]+)?)/)) {
			result = new PathToken(PathToken.NUMBER, parseFloat(RegExp.$1));
			d = d.substr(RegExp.$1.length);
		} else {
			throw new Error("PathLexer.getNextToken: unrecognized path data " + d);
		}
	}
	this._pathData = d;
	return result;
};
PathToken.UNDEFINED = 0;
PathToken.COMMAND = 1;
PathToken.NUMBER = 2;
PathToken.EOD = 3;

function PathToken(type, text) {
	if (arguments.length > 0) {
		this.init(type, text);
	}
}
PathToken.prototype.init = function (type, text) {
	this.type = type;
	this.text = text;
};
PathToken.prototype.typeis = function (type) {
	return this.type == type;
}

return PathParser;
});
