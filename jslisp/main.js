var fs = require ('fs');

var tokenize = function (sourceString) {	
	return sourceString //.replace (/\s/g,  ' ')
							  .replace (/\(/g, ' ( ')
							  .replace (/\)/g, ' ) ')
							  .trim ()
							  .split   (/\s+/g);
};

/*var parenthesize = function (sourceTokens, list) {
	var token = sourceTokens .shift ();

	if (token === undefined) {
		return list .pop ();
	} else
	
	if (token === '(') {
		list .push (parenthesize (sourceTokens, [ ]));
		return parenthesize (sourceTokens, list);
	} else

	if (token === ')') {
		return list;
	} else

	return parenthesize (sourceTokens, list .concat (categorize (token)));
};*/

var accumulateTokens = function (elements, acc, tokensCounter) {	
	if (elements .length === 0)
		return acc;

	var token = elements [0];

	if (token === '(') {
		var result = accumulateTokens (elements .slice (1), [ ], 1);
		return accumulateTokens (
			elements .slice (result .tokensCounter),
			acc .concat ([ result .accumulator ]),
			tokensCounter + result .tokensCounter);
	};
	
	if (token === ')') {
		return {
			tokensCounter : tokensCounter + 1,
			accumulator: acc
		};
	}

	return accumulateTokens (
		elements .slice (1),
		acc .concat (categorize (token)),
		tokensCounter + 1
	);
};

var categorize = function (input) {	
	if (!isNaN (parseFloat (input))) {
		return { type : 'literal', value : parseFloat (input) };
	} else

	if (input [0] === '"' && input .slice (-1) === '"') {
		return { type : 'literal', value : input .slice (1, -1) };
	} else
	
	return { type : 'identifier', value : input };
};

var Context = {
	get : function (identifier) {
		if (identifier in this .scope) {
			return this .scope [ identifier ];
		} else
		
		if (this .parent !== undefined) {
			return this .parent .get (identifier);
		} else
		
		throw new Error ('Unknown identifier "' + identifier + '"!');
	}
};

var createContext = function (scope, parent) {
	var obj = Object .create (Context);
	
	obj .scope = scope;
	obj .parent = parent;
	
	return obj;
};

/*var Context = function (scope, parent) {
	this .scope = scope;
	this .parent = parent;
	
	this .get = function (identifier) {
		if (identifier in this .scope) {
			return this .scope [ identifier ];
		} else
		
		if (this .parent !== undefined) {
			return this .parent .get (identifier);
		}
	};
};*/

var library = {
	first : function (x) {
		//if (arguments .length > 1) throw new Error ('Expected 1 argument for first!');
		
		return x [0];
	},
	
	rest : function (x) {
		//if (arguments .length > 1) throw new Error ('Expected 1 argument for first!');
		
		return x .slice (1);
	},
	
	print : function (x) {
		//if (arguments .length > 1) throw new Error ('Expected 1 argument for first!');
		
		console .log (x);
		return x;
	}
};

var interpret = function (input, context) {
	/*if (context === undefined) {
		return interpret (input, createContext (library));
	} else*/

	if (input instanceof Array) {
		return interpretList (input, context);
	} else

	if (input .type === 'identifier') {
		return context .get (input .value);
	} else
	
	return input .value;
};

var interpretList = function (input, context) {
	if (input .length > 0 && input [0] .value in special) {
		return special [input [0] .value] (input, context);
	} else {
		var list = input .map (function (x) { return interpret (x, context); });
		if (list [0] instanceof Function) {
			return list [0] .apply (undefined, list .slice (1));
		} else {
			return list;
		}
	}
};

var special = {
	/* form of (lambda (params) expr) */
	lambda : function (input, context) {		
		return function (/* arguments */) {
			var lambdaArguments = arguments;
			var lambdaScope = input [1] .reduce (function (acc, x, i) {
				acc [x .value] = lambdaArguments [i];
				return acc;
			}, { });
			
			return interpret (input [2], createContext (lambdaScope, context));
		};
	}
};

var processedString =
	accumulateTokens (
		tokenize (
			fs .readFileSync (
				'test.lisp',
				{ encoding : 'utf-8' })),
		[ ],
		0);

console .log (interpret (processedString, createContext (library)));
