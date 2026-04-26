integral arr = [1, 2, 3, 4, 5];
integral n = 5;

integral i = 1;

while (i < n) {
    arr[i] = arr[i] + arr[i - 1];
    i = i + 1;
}