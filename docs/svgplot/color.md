# SVGPlot colors

There are named attributes for different plot types that require a color. There are several ways to pass a color to them:

- **Typed named colors.** There are a set of SVG colors that are defined as typed variables, that correspond to SVG named colors: `svg::cyan`, `svg::pink`, `svg::red`, `svg::black`, `svg::gray`, `svg::yellow`, `svg::white`, `svg::green`, `svg::blue`, `svg::magenta`, `svg::purple` and `svg::lime`.
- **String named colors.** The same colors as above, but from a string without the `svg::` prefix (e.g. `"green"`).
- **RGB colors.** RGB tuple (between 0 and 255) represented either as a typed color `svg::rgb(128,128,128)` or as a string with parenthesis `"(128,128,128)"`
- **HSV colors.** HSV tuple, where H (hue) is in degrees between 0 and 359, and S (saturation) and V (value) are between 0 and 1. This is only possible as a typed color, such as `svg::hsv(160,1,1)`, and not as a string.
- **Hexadecimal HTML-like colors.** String that represent a color in hexadecimal representation, such as `"#afcaae"`.
- **Abbreviated color strings.** There are also (string) abreviations for most used colors: `"r"` for red, `"g"` for green, `"b"` for blue, `"c"` for cyan, `"m"` for magenta, `"y"` for yellow, `"k"` for black and `"w"` for white.
