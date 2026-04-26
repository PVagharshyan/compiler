integral arr = [1, 2, 3, 4, 5];
integral n = 5;

integral i = 0;
integral j = n - 1;

while (i < j) {

    integral temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;

    i = i + 1;
    j = j - 1;
}