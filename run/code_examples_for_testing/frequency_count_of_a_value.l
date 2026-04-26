integral arr = [2, 3, 2, 5, 2];
integral n = 5;

integral target = 2;
integral i = 0;
integral count = 0;

while (i < n) {

    if (arr[i] == target) {
        count = count + 1;
    }

    i = i + 1;
}