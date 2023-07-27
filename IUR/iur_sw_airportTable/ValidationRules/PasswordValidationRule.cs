using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace iur_sw_airportTable.ValidationRules
{
    public class PasswordValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            string str = (string)value;
            bool okLenght = str.Length > 4;
            bool okFormat = Regex.IsMatch(str, @"[0-9]+");

            if (okLenght && okFormat)
            {
                return ValidationResult.ValidResult;
            }
            return new ValidationResult(false, "Only numbers in password are allowed!");
        }
    }
}
