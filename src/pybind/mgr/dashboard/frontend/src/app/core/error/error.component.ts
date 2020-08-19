import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';

@Component({
  selector: 'cd-error',
  templateUrl: './error.component.html',
  styleUrls: ['./error.component.scss']
})
export class ErrorComponent implements OnInit {
  header: string;
  message: string;
  section: string;
  section_info: string;
  icon: string;

  constructor(public route: ActivatedRoute, private router: Router) {}

  ngOnInit() {
    try {
      this.message = history.state.message;
      this.header = history.state.header;
      this.section = history.state.section;
      this.section_info = history.state.section_info;
      this.icon = history.state.icon;
    } catch (error) {
      this.router.navigate(['error']);
    }
  }
}
