//Kyle Zhang | kmzhang

// Standard libraries
#include <stdio.h>
#include <math.h>
#include <xc.h>

//User libraries:
#include "BOARD.h"


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Divide(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Average(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand);
double Tangent(double operand);
double Round(double operand);

void CalculatorRun(void)
{
    printf("\n\nWelcome to Kyle's calculator program! Compiled at %s %s\n", __DATE__, __TIME__);
    //initialize input, operands, result
    char input;
    double op1, op2, result;
    //infinite loop to prompt the user after their result is printed
    while (1) {
        //reset operands and the result
        op1 = 0;
        op2 = 0;
        result = 0;
        printf("Enter a mathematical operation to perform(*,/,+,-,m,a,c,f,t,r): \n");
        scanf(" %c", &input);
        //checks whether operator is valid
        while ((input != '*') && input != '/' && input != '+' && input != '-' && input != 'm' &&
                input != 'a' && input != 'c' && input != 'f' && input != 't' && input != 'r') {
            printf("Error, not a valid operator\n\n");
            printf("Enter a mathematical operation to perform(*,/,+,-,m,a,c,f,t,r): \n");
            scanf(" %c", &input); //prompt user again
        }
        //check to see whether 1 or 2 operands are needed
        if ((input == 'a') || input == 'c' || input == 'f' || input == 't' || input == 'r') {
            printf("Enter the first operand: \n");
            scanf(" %lf", &op1);
        } else {
            printf("Enter the first operand: \n");
            scanf(" %lf", &op1);
            printf("Enter the second operand: \n");
            scanf(" %lf", &op2);
        }
        //series of if's determine which function to call based on operator
        //echos back what function they wanted to perform and the operands they entered
        if (input == '*') {
            printf("You input %f * %f\n", op1, op2);
            result = Multiply(op1, op2);
            printf("Result of (%f * %f): %.6f\n", op1, op2, result);
        //if the user inputs 0 as the second operand for 0 it tells them zero error
        } else if (input == '/') {
            if (op2 == 0) {
                printf("Divide by zero error!\n");
            } else {
                printf("You input %f / %f\n", op1, op2);
                result = Divide(op1, op2);
                printf("Result of (%f / %f): %.6f\n", op1, op2, result);
            }
        } else if (input == '+') {
            printf("You input %f + %f\n", op1, op2);
            result = Add(op1, op2);
            printf("Result of (%f + %f): %.6f\n", op1, op2, result);
        } else if (input == '-') {
            printf("You input %f - %f\n", op1, op2);
            result = Subtract(op1, op2);
            printf("Result of (%f - %f): %.6f\n", op1, op2, result);
        } else if (input == 'm') {
            printf("You input Average(%f and %f)\n", op1, op2);
            result = Average(op1, op2);
            printf("Result of Average(%f and %f): %.6f\n", op1, op2, result);
        } else if (input == 'a') {
            printf("You input |%f|\n", op1);
            result = AbsoluteValue(op1);
            printf("Result of |%f|: %.6f\n", op1, result);
        } else if (input == 'c') {
            printf("You input Celsius of %f\n", op1);
            result = FahrenheitToCelsius(op1);
            printf("Result of Celsius of %f: %.6fC\n", op1, result);
        } else if (input == 'f') {
            printf("You input Fahrenheit of %f\n", op1);
            result = CelsiusToFahrenheit(op1);
            printf("Result of Fahrenheit of %f: %.6fF\n", op1, result);
        } else if (input == 't') {
            printf("You input tan(%f)\n", op1);
            result = Tangent(op1);
            printf("Result of tan(%f): %.6f\n", op1, result);
        } else {
            printf("You input Round(%f)\n", op1);
            result = Round(op1);
            printf("Result of Round(%f): %.6f\n", op1, result);
        }
    }
    while (1);
}

double Add(double operand1, double operand2)
{
    double a, b;
    a = operand1;
    b = operand2;
    return a + b;
}

double Subtract(double operand1, double operand2)
{
    double a, b;
    a = operand1;
    b = operand2;
    return a - b;
}

double Multiply(double operand1, double operand2)
{
    double a, b;
    a = operand1;
    b = operand2;
    return a * b;
}

double Divide(double operand1, double operand2)
{
    double a, b;
    a = operand1;
    b = operand2;
    return a / b;
}
//checks whether a number is negative and multiplies it by -1 if it is

double AbsoluteValue(double operand)
{
    double a;
    a = operand;
    if (a < 0) {
        a *= -1;
    } else {
        a = operand;
    }
    return a;
}

double FahrenheitToCelsius(double operand)
{
    double a, b;
    a = operand;
    b = (5.0 / 9.0)*(a - 32.0);
    return b;
}

double CelsiusToFahrenheit(double operand)
{
    double a, b;
    a = operand;
    b = ((9.0 / 5.0) * a) + 32;
    return b;
}

double Average(double operand1, double operand2)
{
    double a, b;
    a = operand1;
    b = operand2;
    a += b;
    a /= 2;
    return a;
}
//converts degrees to radians for the math.h tan function to work correctly
double Tangent(double operand)
{
    double a, b;
    a = operand;
    b = a * (M_PI / 180);
    b = tan(b);
    return b;
}
//checks whether the operand is -/+ and then if the decimal part of the number is >0.5

double Round(double operand)
{
    double a, c;
    int b;
    a = operand;
    b = a;
    c = a - b;
    if (c > 0) {
        if (c > 0.5) {
            b = a;
            b += 1;
        } else {
            b = a;
        }
    } else {
        if (c < -0.5) {
            b = a;
            b -= 1;
        } else {
            b = a;
        }
    }
    return b;
}


