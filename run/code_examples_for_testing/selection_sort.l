integral arr = [8, 4, 2, 9, 5];
integral n = 5;

integral i = 0;

while (i < n - 1) {

    integral min_idx = i;
    integral j = i + 1;

    while (j < n) {

        if (arr[j] < arr[min_idx]) {
            min_idx = j;
        }

        j = j + 1;
    }

    integral temp = arr[i];
    arr[i] = arr[min_idx];
    arr[min_idx] = temp;

    i = i + 1;
}