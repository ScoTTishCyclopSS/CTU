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
    public class LoginValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            string str = (string)value;
            bool okLenght = str.Length > 5;
            bool okFormat = Regex.IsMatch(str, @"[a-z0-9]+");

            if (okLenght && okFormat)
            {
                return ValidationResult.ValidResult;

            }
            return new ValidationResult(false, "Login contains only lower case letters and numbers!");
        }
    }
}
