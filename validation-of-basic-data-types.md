# Validations for basic data types


Full name

    String. Length: 4-100. Must contain first and last name

Email address

    String. Max length 254. Validate by confirmation email.
    Also add light front-end validation.

    Or check http://blog.mailgun.com/post/free-email-validation-api-for-web-forms/?utm_source=adroll&utm_medium=fb-nf&utm_content=guardpost

Finnish IBAN

    String. Must be Finnish account. Finnish IBAN is 18 characters.
    http://fi.wikipedia.org/wiki/IBAN
    Validate: http://tarkistusmerkit.teppovuori.fi/tarkmerk.htm#iban

Bitcoin address

    String. Bitcoinaddress. http://rosettacode.org/wiki/Bitcoin/address_validation
    Note: bitcoinaddress length may vary!
    http://bitcoin.stackexchange.com/questions/2564/how-to-validate-a-bitcoin-address-is-a-real-one
