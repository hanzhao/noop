function fib(x) {
  if (x > 1) {
    fib(x - 1) + fib(x - 2);
  } else if (x == 1) {
    1;
  } else {
    0;
  }
}
var n = parseInt(console.read())
console.log('The ', n, 'th num of fibnacci is: ', fib(n));
