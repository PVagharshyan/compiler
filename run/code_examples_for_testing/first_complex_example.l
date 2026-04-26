integral n = 5;
integral i = 0;
integral sum = 0;
integral arr = [1, 2, 3, 4, 5];

for (i = 0; i < n; i = i + 1) {
    arr[i] = arr[i] * 2;
}

i = 0;
while (i < n) {
    sum = sum + arr[i];
    i = i + 1;
}

if (sum > 20) {
    sum = sum - 10;

    if (sum > 15) {
        sum = sum + 5;
    } else {
        sum = sum + 1;
    }

} else {
    sum = 0;
}

arr[2] = sum;

integral result = arr[0] + arr[1] + arr[2];