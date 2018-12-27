type state = {repoData: option(array(RepoData.repo))};
type action =
  | Loaded(array(RepoData.repo));

let dummyRepos: array(RepoData.repo) = [|
  {
    stargazers_count: 27,
    full_name: "jsdf/reason-react-hacker-news",
    html_url: "https://github.com/jsdf/reason-react-hacker-news",
  },
  {
    stargazers_count: 93,
    full_name: "reasonml/reason-tools",
    html_url: "https://github.com/reasonml/reason-tools",
  },
|];

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState: () => {repoData: None},
  didMount: self => {
    let handleReposLoaded = repoData => self.send(Loaded(repoData));

    RepoData.fetchRepos()
    |> Js.Promise.then_(repoData => {
         handleReposLoaded(repoData);
         Js.Promise.resolve();
       })
    |> ignore;
    /* ReasonReact.NoUpdate; */
  },
  reducer: (action, _state) =>
    switch (action) {
    | Loaded(loadedRepos) =>
      ReasonReact.Update({repoData: Some(loadedRepos)})
    },
  render: self => {
    let loadReposButton =
      <button onClick={_event => self.send(Loaded(dummyRepos))}>
        {ReasonReact.string("Load repos")}
      </button>;
    let repoItems =
      switch (self.state.repoData) {
      | Some(repos) =>
        ReasonReact.array(
          Array.map(
            (repo: RepoData.repo) => <RepoItem key={repo.full_name} repo />,
            repos,
          ),
        )
      | None => loadReposButton
      };
    <div className="App">
      <h1> {ReasonReact.string("Reason Projects")} </h1>
      repoItems
    </div>;
  },
};
