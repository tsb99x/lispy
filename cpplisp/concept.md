- atom is a sequence of letters, like `foo`;
- expression is an atom or list of zero or more expressions;
- if expression is a list, then first element is operator and rest is arguments;
- six primitive operators: quote, atom, eq, car, cdr, cons, cond;

- function is lambda-form list expression;
- function call is a list expression with first element as function;
- if operator of list expression is not one of primitive and the value of f is function,
	then substitute and evaluate function;
