integral b = 20; 
integral a = [1,2,3]; 
integral x = 1; 
integral y = 1;
 
y = a[1]; 
a[1] = b;
a[2] = b + 10 + 3*(10 + a[1]);

if (x == 10) { 
    y = x + 5; 
} 
else { 
    y = 0; 
}