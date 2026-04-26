integral arr = [10, 4, 7, 2, 15];
integral n = 5;

integral i = 1;
integral max = arr[0];

while (i < n) {

    if (arr[i] > max) {
        max = arr[i];
    }

    i = i + 1;
}