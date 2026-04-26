integral arr = [7, 3, 5, 2, 9, 1];
integral n = 6;

integral i = 1;

while (i < n) {

    integral key = arr[i];
    integral j = i - 1;

    while (j >= 0 && arr[j] > key) {

        arr[j + 1] = arr[j];
        j = j - 1;

    }

    arr[j + 1] = key;

    i = i + 1;
}