var a = 0, b = 1, c = 1, i = 1;
while (i < 10) {
  c = a + b;
  a = b;
  b = c;
  i = i + 1;
}
console.log('The 10th num of fibnacci is: ' + c);
