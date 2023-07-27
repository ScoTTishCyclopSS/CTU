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
    public class TextBoxValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            string str = (string)value;
            bool okLenght = str.Length > 0;
            bool okFormat = Regex.IsMatch(str, @"[a-zA-Z0-9 ]+");

            if (okLenght && okFormat)
            {
                return ValidationResult.ValidResult;
            }
            return new ValidationResult(false, "Too short or unsuported characters!");
        }
    }
}
