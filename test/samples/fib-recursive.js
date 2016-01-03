var a = 0, b = 1, c = 1;
function fib(x) {
  if (x > 1) {
    fib(x - 1);
    c = a + b;
    a = b;
    b = c;
  }
}
fib(10);
console.log('The 10th num of fibnacci is: ' + c);
