integral arr = [1, 2, 3, 4, 6, 7];
integral n = 6;

integral i = 0;
integral count = 0;

while (i < n) {

    if ((arr[i] / 2) * 2 == arr[i]) {
        count = count + 1;
    }

    i = i + 1;
}