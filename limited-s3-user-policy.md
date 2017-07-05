# Creating a S3 bucket and IAM user

We want:

* S3 bucket
* IAM user which has access keys and very limited right to use the bucket

1. Go to AWS console and IAM page > Users tab

2. Add user e.g. `myproject-s3-admin`

    * Give it only "Programmatic access" meaning key id and secret access key
    * Don't give the user any permissions
    * Create user and save the credentials

3. Go to the user details and save its ARN

    Looks something like `arn:aws:iam::282299233989:user/myproject-s3-admin`

4. Go to S3 page. Create bucket.

    * Name it well, describing the env e.g. `myproject-backups-prod` to the region of your choosing
    * Optionally enable versioning, logging or tags
    * Make sure it's not publicly available if you don't want to

5. Go to bucket details and Permissions tab
6. Click Bucket policy, it'll show you an editor. Copy paste the template below and replace `<BUCKET_NAME>` and `<USER_ARN>` with the ones you have.

7. You now have a S3 bucket, and a user which has only a limited permissions to use AWS.

    The IAM user can only

    * List files in your bucket
    * Get/put objects to the bucket, meaning downloading and uploading files


### S3 policy document template

```json
{
    "Version": "2008-10-17",
    "Id": "Give limited S3 access to user",
    "Statement": [
        {
            "Sid": "S3 list access for external user",
            "Effect": "Allow",
            "Principal": {
                "AWS": "<USER_ARN>"
            },
            "Action": "s3:ListBucket",
            "Resource": "arn:aws:s3:::<BUCKET_NAME>"
        },
        {
            "Sid": "S3 object access for external user",
            "Effect": "Allow",
            "Principal": {
                "AWS": "<USER_ARN>"
            },
            "Action": [
                "s3:GetObject",
                "s3:PutObject"
            ],
            "Resource": "arn:aws:s3:::<BUCKET_NAME>/*"
        }
    ]
}
```