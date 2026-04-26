integral arr = [1, 2, 3, 5, 4];
integral n = 5;

integral i = 0;
integral sorted = 1;

while (i < n - 1) {

    if (arr[i] > arr[i + 1]) {
        sorted = 0;
        i = n;
    }

    i = i + 1;
}