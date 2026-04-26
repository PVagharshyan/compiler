integral arr = [1, 2, 3, 4];
integral n = 4;

integral last = arr[n - 1];

integral i = n - 1;

while (i > 0) {
    arr[i] = arr[i - 1];
    i = i - 1;
}

arr[0] = last;