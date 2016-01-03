function fib(x) {
  if (x > 1) {
    fib(x - 1) + fib(x - 2);
  } else if (x == 1) {
    1;
  } else {
    0;
  }
}
console.log('The 10th num of fibnacci is: ' + fib(10));
