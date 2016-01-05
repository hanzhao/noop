var val = 5;
var arr = [1, 2, 3, val];
arr[6] = "end";
var i = 0;
var len = arr.length;
while (i < len) {
  console.log(arr[i]);
  i = i + 1;
}
console.log(arr);
