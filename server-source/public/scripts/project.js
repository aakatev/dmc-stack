// Hide/unhide menu
let i = 0;
nav_status = ['grid','none'];
show_hide_navbar = () => {
  document.getElementsByClassName("navbar")[0].style.display = nav_status[i%2];
  i++; 
}
 
