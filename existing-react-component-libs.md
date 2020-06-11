# Existing React component libraries

Some of the projects are huge, the link most commonly points directly to the part of the code base which defines the components. For tests, I checked that tests for a few components look OK, no coverage or deeper analysis done.


Next things to consider:

* Is treeshaking possible when using just parts of the library?
* Maturity and activeness (commits, recent activity, contributor distribution, closed vs open issues, PR merging activity)
* Are they migrating to another tech ? JS -> TS, Sass -> styled-components, etc
* Owners, is it community driven or company driven? Project with 13 stars can be super mature if it's been developed internally in a company for 20k commits for N years


Link | Commits* | JS/TS | Styling | License | Tests | Other
-----|---------|-------|---------|---------|-------|-------
[React Bootstrap](https://github.com/react-bootstrap/react-bootstrap/tree/master/src)                           | 3.8k  | JS | Bootstrap CSS classes | MIT (also Bootstrap is MIT) | ☑️
[Zendesk Garden](https://github.com/zendeskgarden/react-components/tree/master/packages)                        | 0.8k  | TS | styled-components | Apache 2.0  | ☑️ | ☑️ Storybook
[Material UI](https://github.com/mui-org/material-ui/tree/master/packages/material-ui/src)                      | 11.9k | JS | CLSX, CSS in JS. [Example](https://github.com/mui-org/material-ui/blob/edb2d6382e271dbfc28384b10c417c0f5843e8f8/packages/material-ui/src/Button/Button.js#L9)            | MIT | ☑️
[RMWC](https://github.com/jamesmfriedman/rmwc/tree/master/src)                                                  | 1.9k  | JS | CSS classnames using [material-components-web](https://github.com/material-components/material-components-web/tree/master/packages/material-components-web) by Google (source is Sass) | MIT (also material-components-web is MIT) | ☑️
[Reactstrap](https://github.com/reactstrap/reactstrap/tree/master/src)                                          | 0.8k  | JS | Bootstrap CSS classes | MIT | ☑️
[Fluent UI by Microsoft](https://github.com/microsoft/fluentui/tree/master/packages/office-ui-fabric-react/src) | 8.6k  | TS | CSS in JS, seems to be a [custom one](https://github.com/microsoft/fluentui/blob/master/packages/utilities/src/styled.tsx) | MIT | ☑️
[Ant Design](https://github.com/ant-design/ant-design/tree/master/components)                                   | 18.6k | TS | Less. [example](https://github.com/ant-design/ant-design/blob/master/components/card/style/index.less) | MIT | ☑️
[Carbon](https://github.com/carbon-design-system/carbon/tree/master/packages/react/src)                         | 6.8k  | JS | CSS classnames linking to [Sass](https://github.com/carbon-design-system/carbon/blob/master/packages/components/src/components/button/_button.scss) | Apache 2.0 | ☑️ | ☑️ Storybook
[Grommet](https://github.com/grommet/grommet/blob/master/src/js/components/)                                    | 5.2k  | JS | styled-components | Apache 2.0 | ☑️ | ☑️ Storybook
[Aragon UI](https://github.com/aragon/aragon-ui/blob/master/src/components/)                                    | 0.8k  | JS | styled-components | MIT | [Not much](https://github.com/aragon/aragon-ui/search?q=test&unscoped_q=test)
[Blueprint](https://github.com/palantir/blueprint/tree/develop/packages/core/src/components)                    | 1.8k  | TS | Sass | Apache 2.0 | ☑️
[Basis](https://github.com/moroshko/basis/blob/master/src/components)                                           | 0.3k  | JS | emotion | MIT | ☑️
[MongoDB design](https://github.com/mongodb/design/tree/master/client/react-components)                         | 0.4k  | JS | CSS | package.json says MIT, no LICENSE file | [No](https://github.com/mongodb/design/search?q=test&unscoped_q=test)
[Salesforce Design System](https://github.com/salesforce-ux/design-system/tree/master/ui/components)            | 9.5k  | JS | Sass | Code BSD 3-Clause, see: https://github.com/salesforce-ux/design-system#licenses | ☑️
[Pivotal UI](https://github.com/pivotal-cf/pivotal-ui/tree/master/src/react)                                    | 4.0k  | JS | Sass | MIT | ☑️
[Primer by GitHub](https://github.com/primer/components/tree/master/src)                                        | 4.5k  | JS | styled-components | MIT | ☑️
[Polaris by Shopify](https://github.com/Shopify/polaris-react/tree/master/src/components)                       | 6.1k  | TS | Sass | Modified MIT, [see more](https://github.com/Shopify/polaris-react#licenses) |  ☑️
[Baseweb by Uber](https://github.com/uber/baseweb/tree/master/src)                                              | 2.3k  | JS (Flow) | [style-tron](https://github.com/styletron/styletron) | MIT | ☑️
[Thumbprint](https://github.com/thumbtack/thumbprint/tree/master/packages/thumbprint-react/components)          | 0.7k  | TS | Sass | Apache 2.0 |  ☑️
[VTEX Styleguide](https://github.com/vtex/styleguide/tree/master/react)                                         | 4.3k  | JS | Custom [Tachyons](https://vtex.github.io/vtex-tachyons/) classes + CSS. Example [implementation](https://github.com/vtex/styleguide/blob/master/react/components/Input/index.js) and [CSS](https://github.com/vtex/styleguide/blob/master/react/components/Input/Input.css) | [Unlicensed](https://github.com/vtex/styleguide/issues/625) | [Not much](https://github.com/vtex/styleguide/issues/1083)

* Most of these repos are monorepos managed with Lerna. Commit count is just to give rough estimate of how much the code has been iterated.


For further investigation, see how many Button components there are in GitHub: https://github.com/search?l=&p=99&q=created%3A%3E2015-01-01+extension%3Ajs+extension%3Ajsx+extension%3Ats+extension%3Atsx+size%3A%3E100+filename%3AButton.js+filename%3AButton.ts+filename%3AButton.jsx+filename%3AButton.tsx+filename%3Abutton.js+filename%3Abutton.ts+filename%3Abutton.jsx+filename%3Abutton.tsx&ref=advsearch&type=Code
