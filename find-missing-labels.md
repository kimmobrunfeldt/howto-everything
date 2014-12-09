**find-missing-labels.js**

```javascript
var requirejs = require('requirejs');
var _ = require('underscore');
var Set = require("collections/set");

// Finds which labels are missing from languages. Assuming that union of
// the languages' keys is the correct key space.
function keysMissing(localization) {
    var allLabelIdentifiers = new Set();

    _.each(localization, function(translations, language) {
        _.each(translations, function(val, labelIdentifier) {
            allLabelIdentifiers.add(labelIdentifier);
        });
    });

    var missing = {};

    _.each(localization, function(translations, language) {
        var missingKeys = [];

        _.each(allLabelIdentifiers.toArray(), function(labelIdentifier) {
            if (!_.has(translations, labelIdentifier)) {
                missingKeys.push(labelIdentifier);
            }
        });
        missing[language] = missingKeys;
    });

    return missing;
}

var localizationJson = {"fi":{"error.login":"Ei pysty kirjautumaan","error.any":"Virhe"},"sv":{"error.car":"En bil inte"},"en":{"error.login":"Cannot login"}};

_.each(keysMissing(localizationJson), function(labelIdentifiers, language) {
    if (labelIdentifiers.length === 0) {
        return;
    }

    console.log('Missing labels for language ' + language + ':');
    _.each(labelIdentifiers, function(labelIdentifier) {
        console.log('- ' + labelIdentifier);
    });
    console.log('');
});
***


**find_missing_labels.py**

```python
"""
Takes a json label localization data and reports missing labels for each
language.
"""

import json


# Test data
localization_json = """
{
    "fi": {
        "error.login": "Ei pysty kirjautumaan",
        "error.any": "Virhe"
    },
    "sv": {
        "error.car": "En bil inte"
    },
    "en": {
        "error.login": "Cannot login"
    }
}
"""


def keys_missing(localization_dict):
    """Finds which labels are missing from languages. Assuming that union of
    the languages' keys is the correct key space."""
    all_labels = set()
    for language, translations in localization_dict.items():
        for label in translations:
            all_labels.add(label)

    missing = {}
    for language, translations in localization_dict.items():
        missing_keys = []
        for label in all_labels:
            if label not in translations:
                missing_keys.append(label)
        missing[language] = missing_keys

    return missing


def main():
    localization_dict = json.loads(localization_json)
    missing = keys_missing(localization_dict)

    print(missing)


if __name__ == '__main__':
    main()
```
