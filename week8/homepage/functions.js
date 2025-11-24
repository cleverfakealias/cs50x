function loadHeader() {
  fetch("header.html")
    .then((response) => response.text())
    .then((data) => {
      document.querySelector(".headerHtml").innerHTML = data;
    })
    .catch((error) => console.error("Error loading header:", error));
}
