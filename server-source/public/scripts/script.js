function loadWebAssembly(fileName) {
  return fetch(fileName)
    .then(response => response.arrayBuffer())
    .then(bits => WebAssembly.compile(bits))
    .then(module => { return new WebAssembly.Instance(module) });
};

let a = parseInt(Math.random()*100), b = parseInt(Math.random()*100);

loadWebAssembly('scripts/minmax.wasm')
  .then(instance => {
    console.log(" Your lucky numbers are " +a+" and "+b+ ". Smallest is " + instance.exports.min(a,b) +", while biggest is "+instance.exports.max(a,b)+".");
});


let i = 0;
nav_status = ['grid','none'];
show_hide_navbar = () => {
  document.getElementsByClassName("navbar")[0].style.display = nav_status[i%2];
  i++; 
}
 
const fetchAndLog = async () => {
  const response = await fetch('http://gateway.marvel.com/v1/public/comics?ts=1&apikey=1234&hash=ffd275c5130566a2916217b101f26150');
  const json = await response.json();
  console.log(json[0]);

}

fetchAndLog();