function func() {
  var a = [1, 2, 3, 4, 5, 6];
}
var i = 0;
while (i < 10000) {
  func();
  i = i + 1;
}
console.log(i);
