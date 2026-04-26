integral arr = [3, 7, 1, 9, 5];
integral n = 5;

integral target = 9;
integral i = 0;
integral found = 0;

while (i < n) {

    if (arr[i] == target) {
        found = 1;
        i = n;
    }

    i = i + 1;
}