integral n = 10;
integral arr = [9, 1, 8, 2, 7, 3, 6, 4, 5, 0];
integral temp = 0;
integral i = 0;
integral j = 0;

for (i = 0; i < n; i = i + 1) {
    for (j = 0; j < n - 1; j = j + 1) {
        if (arr[j] > arr[j + 1]) {
            temp = arr[j];
            arr[j] = arr[j + 1];
            arr[j + 1] = temp;
        } else {
            temp = 0;
        }
    }
}

integral sum = 0;
i = 0;

while (i < n) {
    sum = sum + arr[i];

    if (arr[i] > 5) {
        sum = sum + 10;
    } else {
        sum = sum + 1;
    }

    i = i + 1;
}

integral max = arr[0];
i = 1;

while (i < n) {
    if (arr[i] > max) {
        max = arr[i];
    }
    i = i + 1;
}

if (sum > max) {
    sum = sum - max;
} else {
    sum = sum + max;
}

integral result = sum + max;

if (result > 50) {
    result = result / 2;
} else {
    result = result + 5;
}