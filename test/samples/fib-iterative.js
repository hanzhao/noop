var a = 0, b = 1, c = 1, i = 1, n = parseInt(console.read());
while (i < n) {
  c = a + b;
  a = b;
  b = c;
  i = i + 1;
}
console.log('The ', n, 'th num of fibnacci is: ', c);
