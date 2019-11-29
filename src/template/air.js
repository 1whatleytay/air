function appElement() {
    return document.getElementById("app")
}

function refresh(element) {
    element.element.innerHTML = element.render();
    element.finalize();
}

function loadApp() {
    let mainElement = main();

    mainElement.element = appElement();
    refresh(mainElement);
}

window.onload = loadApp;
