/// Entry point of the program
fn main() i32 {
    i32 result = fibonacci(10);

    if (is_prime(abs(result))) {
            return 1;
    } else {
        return 0;
    }
}

/// Calculates the nth Fibonacci number
fn fibonacci(i32 n) i32 {
    if(n <= 1) {
        return n;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

/// Determines if a number is prime
fn is_prime(i32 n) bool {
    if(n <= 1) {
        return false;
    }

    mut i32 i = 2;
    if(i * i <= n) {
        if(n % i == 0) {
            return false;
        }
        i++;
    }

    return true;
}

/// Calculates the absolute value of a number
fn abs(i32 x) i32 {
    if(x < 0) {
        return -x;
    } else {
        return x;
    }
}
