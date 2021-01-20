#ifndef FMT_COMPLEX_H_
#define FMT_COMPLEX_H_


#include <complex>
#include <type_traits>
#include "locale.h"
#include "format.h"


FMT_BEGIN_NAMESPACE

// support for std::complex taken from locale-test.cc in test/
//
// this structure has format enabled only if 
// the template parameter Float satisfies std::is_floating_point
// since std::complex is unspecified for types besides float, double, and long double
template <class charT, class Float>
struct formatter<std::complex<Float>, charT> {
 private:
  detail::dynamic_format_specs<char> specs_;

 public:
  FMT_CONSTEXPR typename basic_format_parse_context<charT>::iterator parse(
      basic_format_parse_context<charT>& ctx) {
    using handler_type =
        detail::dynamic_specs_handler<basic_format_parse_context<charT>>;
    detail::specs_checker<handler_type> handler(handler_type(specs_, ctx),
                                                detail::type::string_type);
    auto it = parse_format_specs(ctx.begin(), ctx.end(), handler);
    detail::parse_float_type_spec(specs_, ctx.error_handler());
    return it;
  }

  template <class FormatContext, FMT_ENABLE_IF(std::is_floating_point<Float>::value)>
  typename FormatContext::iterator format(const std::complex<Float>& c,
                                          FormatContext& ctx) {
    detail::handle_dynamic_spec<detail::precision_checker>(
        specs_.precision, specs_.precision_ref, ctx);
    auto specs = std::string();
    if (specs_.precision > 0) specs = fmt::format(".{}", specs_.precision);
    if (specs_.type) specs += specs_.type;
    auto real = fmt::format(ctx.locale().template get<std::locale>(),
                            "{:" + specs + "}", c.real());
    auto imag = fmt::format(ctx.locale().template get<std::locale>(),
                            "{:" + specs + "}", c.imag());
    auto fill_align_width = std::string();
    if (specs_.width > 0)
      fill_align_width = fmt::format(">{}", specs_.width);

    // build the presentation
    // this additionally allows for 0 + 1i to be displayed as just "i"
    auto presentation = std::string();
    if (c.real() == 0 && c.imag() != 1)
      presentation = "{1}i";
    else if (c.real() == 0 && c.imag() == 1)
      presentation = "i";
    else 
      presentation = "({0}+{1}i)";


    return format_to(
        ctx.out(), "{:" + fill_align_width + "}",
        fmt::format(presentation, real, imag));
  }
};

FMT_END_NAMESPACE


#endif